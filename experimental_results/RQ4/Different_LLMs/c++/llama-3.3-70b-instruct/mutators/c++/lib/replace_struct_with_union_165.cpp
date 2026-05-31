//source file
#include "../include/Replace_Struct_With_Union_165.h"

// ========================================================================================================
#define MUT165_OUTPUT 1

void MutatorFrontendAction_165::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::RecordDecl>("StructDecls")) {
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
      std::string mutatedDeclaration = declaration.str();
      size_t pos = mutatedDeclaration.find("struct");
      if (pos != std::string::npos) {
        mutatedDeclaration.replace(pos, 6, "union");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
std::unique_ptr<clang::ASTConsumer> MutatorFrontendAction_165::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) {
    clang::Rewriter TheRewriter;
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return std::make_unique<MutatorASTConsumer_165>(TheRewriter);
}

void MutatorFrontendAction_165::MutatorASTConsumer_165::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    clang::ast_matchers::DeclarationMatcher matcher = clang::ast_matchers::recordDecl(clang::ast_matchers::isStruct()).bind("StructDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}