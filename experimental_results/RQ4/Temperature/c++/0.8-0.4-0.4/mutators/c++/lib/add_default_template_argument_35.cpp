//source file
#include "../include/add_default_template_argument_35.h"

// ========================================================================================================
#define MUT35_OUTPUT 1

void MutatorFrontendAction_35::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;

      // Check if the template parameter already has a default argument
      bool hasDefaultArgument = false;
      for (const auto *param : *FTD->getTemplateParameters()) {
          if (const auto *typeParam = llvm::dyn_cast<clang::TemplateTypeParmDecl>(param)) {
              if (typeParam->hasDefaultArgument()) {
                  hasDefaultArgument = true;
                  break;
              }
          }
      }

      if (!hasDefaultArgument) {
          //Get the source code text of target node
          auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                     FTD->getSourceRange());
          //Perform mutation on the source code text by applying string replacement
          size_t pos = declaration.find("typename");
          if (pos != std::string::npos) {
              declaration.insert(pos + 8, " = int");
          }
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(FTD->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_35::MutatorASTConsumer_35::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}