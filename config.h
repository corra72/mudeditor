
#ifndef TS_CONFIG_H
#define TS_CONFIG_H

#include <qstring.h>
#include "version.h"

namespace TS
{
  const QString PROGRAM_NAME = "TS Kreator";
  const QString MESSAGE_BOX_INFORMATION = "Informazione - TS Kreator";
  const QString MESSAGE_BOX_WARNING = "Attenzione - TS Kreator";
  const QString MESSAGE_BOX_QUESTION = "Domanda - TS Kreator";
  const QString MESSAGE_BOX_CRITICAL = "Errore - TS Kreator";
  const QString MESSAGE_BOX_ABOUT = "About - TS Kreator";
  const QString MESSAGE_ASK_SAVE = "Sono state rilevate alcune modifiche che non sono state salvate.\nVolete applicare i cambiamenti adesso?";
  const QString MESSAGE_ASK_OVERWRITE_VNUMBER = "Esiste gia' un'entita' con VNumber %1. Volete sovrascriverla?";
  const QString MESSAGE_ASK_REMOVE_VNUMBER = "Vuoi realmente eliminare l'entita' con VNumber %1?";
  const QString MESSAGE_ASK_REMOVE_REFERENCES = "Vuoi realmente eliminare tutti i riferimenti al VNumber %1?";
  const QString MESSAGE_ASK_UPDATE_REFERENCES = "Vuoi aggiornare i riferimenti dal VNumber %1 al VNumber %2?";
  const QString MESSAGE_WARNING_NO_VNUMBER = "Non e' stato possibile trovare l'entita' con VNumber %1.\nProbabilmente e' stata cancellata.";
  const QString MESSAGE_WARNING_NO_EXIT = "Non e' stato possibile trovare l'uscita '%1'.\nProbabilmente e' stata cancellata.";
  const QString MESSAGE_INVALID_STRING = "La stringa non puo' essere vuota e non deve contenere il carattere ~ (ALT+126).";
  const QString MESSAGE_WARNING_CANT_SAVE_ON_FILE = "Non e' stato possibile scrivere sul file:\n%1";
  const QString MESSAGE_WARNING_FIELD_IS_EMPTY = "Il campo %1 e' vuoto. Impossibile continuare.";

};

#endif // TS_CONFIG_H
