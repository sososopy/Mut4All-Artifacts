//source file
#include "../include/Insert_Incomplete_Struct_Definition_181.h"

// ========================================================================================================
#define MUT181_OUTPUT 1

void MutatorFrontendAction_181::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("RecordDecls")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          clang::LangOptions());
      std::string declarationStr(declaration.begin(), declaration.end());
      //Perform mutation on the source code text by applying string replacement
      std::string insertText = "\n  struct IncompleteStruct;\n";
      size_t pos = declarationStr.find("}");
      if (pos != std::string::npos) {
        declarationStr.insert(pos, insertText);
      } else {
        declarationStr += insertText;
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), declarationStr);
    }
}
  
std::unique_ptr<clang::ASTConsumer> MutatorFrontendAction_181::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef file) {
    clang::Rewriter TheRewriter;
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return std::make_unique<MutatorASTConsumer_181>(TheRewriter);
}

void MutatorFrontendAction_181::MutatorASTConsumer_181::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    clang::ast_matchers::DeclarationMatcher matcher = clang::ast_matchers::cxxRecordDecl().bind("RecordDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}