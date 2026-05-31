//source file
#include "../include/Mutator_Replace_Template_Parameter_With_Void_459.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/Tooling.h"
#include <string>

// ========================================================================================================
#define MUT459_OUTPUT 1

void MutatorFrontendAction_459::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!MT || !SourceManager.isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          SourceManager, 
          Context.getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      std::string templateParameter = "void";
      std::string declarationStr = declaration.str();
      size_t pos = declarationStr.find("typename");
      while (pos != std::string::npos) {
        size_t endPos = declarationStr.find(">", pos);
        if (endPos != std::string::npos) {
          declarationStr.replace(pos + 8, endPos - pos - 8, templateParameter);
        }
        pos = declarationStr.find("typename", pos + 1);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), declarationStr);
    }
}
  
std::unique_ptr<clang::ASTConsumer> MutatorFrontendAction_459::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) {
    clang::Rewriter TheRewriter;
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return std::make_unique<MutatorASTConsumer_459>(TheRewriter);
}

void MutatorFrontendAction_459::MutatorASTConsumer_459::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    clang::ast_matchers::DeclarationMatcher matcher = clang::ast_matchers::classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter, Context, Context.getSourceManager());
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}