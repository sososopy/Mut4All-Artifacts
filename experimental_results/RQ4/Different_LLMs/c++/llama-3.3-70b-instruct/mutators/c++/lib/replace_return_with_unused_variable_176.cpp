//source file
#include "../include/Mutator_Replace_Return_With_Unused_Variable_176.h"

// ========================================================================================================
#define MUT176_OUTPUT 1

void MutatorFrontendAction_176::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      std::string returnType = MT->getReturnType().getAsString();
      std::string mutatedCode = declaration.str();
      size_t pos = mutatedCode.find("return");
      if (pos != std::string::npos) {
        mutatedCode.replace(pos, 6, returnType + " unusedVariable;");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedCode);
    }
}
  
std::unique_ptr<clang::ASTConsumer> MutatorFrontendAction_176::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) {
    clang::Rewriter TheRewriter;
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return std::make_unique<MutatorASTConsumer_176>(TheRewriter);
}

void MutatorFrontendAction_176::MutatorASTConsumer_176::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    clang::ast_matchers::DeclarationMatcher matcher = clang::ast_matchers::functionDecl().bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}