//source file
#include "../include/Replace_Template_Parameter_305.h"

// ========================================================================================================
#define MUT305_OUTPUT 1

void MutatorFrontendAction_305::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Template")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string replacementType = "int"; // default replacement type
      for (auto it = MT->getTemplateParameters()->begin(); it != MT->getTemplateParameters()->end(); ++it) {
        if (auto *tparam = dyn_cast<clang::TemplateTypeParmDecl>(*it)) {
          // Replace the template parameter with a different type
          replacementType = "float"; // example replacement type
          break;
        }
      }
      size_t pos = declaration.find("template <");
      if (pos != std::string::npos) {
        size_t endPos = declaration.find(">", pos);
        if (endPos != std::string::npos) {
          std::string templateParams = declaration.substr(pos + 9, endPos - pos - 9);
          size_t commaPos = templateParams.find(",");
          if (commaPos != std::string::npos) {
            std::string firstParam = templateParams.substr(0, commaPos);
            declaration.replace(pos + 9, firstParam.length(), replacementType);
          } else {
            declaration.replace(pos + 9, templateParams.length(), replacementType);
          }
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_305::MutatorASTConsumer_305::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}