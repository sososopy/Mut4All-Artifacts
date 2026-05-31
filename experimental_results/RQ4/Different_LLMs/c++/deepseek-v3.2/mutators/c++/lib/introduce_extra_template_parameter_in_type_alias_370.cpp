//source file
#include "../include/Introduce_Extra_Template_Parameter_In_Type_Alias_370.h"

// ========================================================================================================
#define MUT370_OUTPUT 1

void MutatorFrontendAction_370::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("TypeAliasTemplateDecl")) {
      //Filter nodes in header files
      if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TA->getLocation()))
        return;
      //Get the source code text of target node
      auto sourceRange = TA->getSourceRange();
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      
      //Collect available template parameter names from the translation unit
      if (availableTemplateParamNames.empty()) {
        auto &context = *Result.Context;
        auto decls = context.getTranslationUnitDecl()->decls();
        for (auto decl : decls) {
          if (auto *templateDecl = dyn_cast<TemplateDecl>(decl)) {
            auto *tpl = templateDecl->getTemplateParameters();
            if (tpl) {
              for (auto param : *tpl) {
                if (auto *tpp = dyn_cast<TemplateTypeParmDecl>(param)) {
                  std::string name = tpp->getNameAsString();
                  if (!name.empty()) {
                    availableTemplateParamNames.push_back(name);
                  }
                }
              }
            }
          }
        }
        //Add a default name if none found
        if (availableTemplateParamNames.empty()) {
          availableTemplateParamNames.push_back("X");
        }
      }
      
      //Choose a random template parameter name
      std::string extraParamName;
      if (availableTemplateParamNames.size() > 0) {
        int index = getrandom::getRandomIndex(availableTemplateParamNames.size() - 1);
        extraParamName = availableTemplateParamNames[index];
      } else {
        extraParamName = "X";
      }
      
      //Perform mutation on the source code text by applying string replacement
      //Find the position after the first template parameter list
      std::size_t templateEndPos = originalText.find("using");
      if (templateEndPos == std::string::npos) return;
      
      //Insert extra template parameter list
      std::string mutatedText = originalText;
      std::string extraTemplate = " template <typename " + extraParamName + ">";
      mutatedText.insert(templateEndPos, extraTemplate);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_370::MutatorASTConsumer_370::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("TypeAliasTemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}