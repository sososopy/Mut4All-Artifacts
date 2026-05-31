//source file
#include "../include/modify_template_argument_list_in_alias_template_30.h"

// ========================================================================================================
#define MUT30_OUTPUT 1

void MutatorFrontendAction_30::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ATD = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("AliasTemplate")) {
      //Filter nodes in header files
      if (!ATD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ATD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto aliasDecl = ATD->getTemplatedDecl();
      if (auto *TTPD = llvm::dyn_cast<clang::TemplateTypeParmDecl>(ATD->getTemplateParameters()->getParam(0))) {
        if (!TTPD->isParameterPack()) {
          //Perform mutation on the source code text by applying string replacement
          auto sourceRange = aliasDecl->getSourceRange();
          auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
          std::string packExpansion = TTPD->getNameAsString() + "...";
          size_t pos = sourceText.find(TTPD->getNameAsString());
          if (pos != std::string::npos) {
            sourceText.replace(pos, TTPD->getNameAsString().length(), packExpansion);
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), sourceText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_30::MutatorASTConsumer_30::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("AliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}