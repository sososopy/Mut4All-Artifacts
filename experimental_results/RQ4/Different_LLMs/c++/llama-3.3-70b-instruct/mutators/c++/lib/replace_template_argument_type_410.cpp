//source file
#include "../include/Replace_Template_Argument_Type_410.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT410_OUTPUT 1

void MutatorFrontendAction_410::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Get the source code text of target node
      auto templateParams = TD->getTemplateParameters();
      for (auto param = templateParams->begin(); param != templateParams->end(); ++param) {
        if (const auto *NTTP = dyn_cast<clang::NonTypeTemplateParmDecl>(*param)) {
          //Perform mutation on the source code text by applying string replacement
          std::string newType = "std::uint64_t";
          std::string declaration = clang::Lexer::getSourceText(
              clang::CharSourceRange::getTokenRange(TD->getSourceRange()), 
              *Result.SourceManager, 
              clang::LangOptions()
          ).str();
          declaration.replace(declaration.find(NTTP->getNameAsString()), NTTP->getNameAsString().length(), newType);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), declaration);
        }
      }
    }
}

void MutatorFrontendAction_410::MutatorASTConsumer_410::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}