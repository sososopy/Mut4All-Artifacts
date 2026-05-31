//source file
#include "../include/TemplateArgumentDeduction_Mutator_428.h"
#include "clang/AST/AST.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include <string>

// ========================================================================================================
#define MUT428_OUTPUT 1

std::unique_ptr<clang::ASTConsumer> MutatorFrontendAction_428::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) {
    clang::Rewriter TheRewriter;
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return std::make_unique<MutatorASTConsumer_428>(TheRewriter);
}

void MutatorFrontendAction_428::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CT = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!CT || !SourceManager.isWrittenInMainFile(
                     CT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(CT->getSourceRange()), 
          SourceManager, 
          Context.getLangOpts());
      std::string strDeclaration = declaration.str();
      //Perform mutation on the source code text by applying string replacement
      // For example, replace new bug_class(0,instantiation) with new bug_class<int>(0,instantiation)
      size_t pos = strDeclaration.find("(");
      if (pos != std::string::npos) {
        strDeclaration.insert(pos, "<int>");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(CT->getSourceRange()), strDeclaration);
    }
}
  
void MutatorFrontendAction_428::MutatorASTConsumer_428::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::callExpr().bind("CallExpr");
    Callback callback(TheRewriter, Context, Context.getSourceManager());
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}