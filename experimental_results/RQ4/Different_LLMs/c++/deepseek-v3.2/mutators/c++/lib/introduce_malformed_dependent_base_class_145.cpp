//source file
#include "../include/Mutator_Introduce_Malformed_Dependent_Base_Class_145.h"

// ========================================================================================================
#define MUT145_OUTPUT 1

void MutatorFrontendAction_145::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
      //Filter nodes in header files
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      
      auto *TemplatedClass = CTD->getTemplatedDecl();
      if (!TemplatedClass || !TemplatedClass->isCompleteDefinition())
        return;
      
      //Get the source code text of target node
      auto SourceRange = TemplatedClass->getSourceRange();
      auto OriginalText = stringutils::rangetoStr(*(Result.SourceManager), SourceRange);
      
      //Find a template parameter to use for the malformed base
      auto *TemplateParams = CTD->getTemplateParameters();
      if (!TemplateParams || TemplateParams->size() == 0)
        return;
      
      //Use the first template parameter
      auto *FirstParam = *TemplateParams->begin();
      std::string ParamName = FirstParam->getNameAsString();
      if (ParamName.empty()) {
        //If parameter has no name, we can't construct T::type
        return;
      }
      
      //Check if class already has base classes
      bool HasBases = TemplatedClass->getNumBases() > 0;
      
      //Perform mutation on the source code text by applying string replacement
      std::string MutatedText;
      std::string ClassName = TemplatedClass->getNameAsString();
      size_t ClassPos = OriginalText.find(ClassName);
      
      if (ClassPos != std::string::npos) {
        //Find the position after class name to insert inheritance
        size_t InsertPos = ClassPos + ClassName.length();
        
        //Look for '{' or ':' after class name
        size_t ColonPos = OriginalText.find(':', InsertPos);
        size_t BracePos = OriginalText.find('{', InsertPos);
        
        if (HasBases && ColonPos != std::string::npos && ColonPos < BracePos) {
          //Class already has bases, prepend our malformed base
          MutatedText = OriginalText;
          std::string NewBase = " public " + ParamName + "::type,";
          MutatedText.insert(ColonPos + 1, NewBase);
        } else {
          //Class has no bases, need to add inheritance specifier
          MutatedText = OriginalText;
          std::string Inheritance = " : public " + ParamName + "::type";
          
          if (BracePos != std::string::npos) {
            MutatedText.insert(BracePos, Inheritance);
          } else {
            //No brace found (maybe forward declaration), can't mutate
            return;
          }
        }
        
        MutatedText = "/*mut145*/" + MutatedText;
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(SourceRange), MutatedText);
      }
    }
}
  
void MutatorFrontendAction_145::MutatorASTConsumer_145::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}