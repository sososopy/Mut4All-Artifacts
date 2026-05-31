//source file
#include "../include/Replace_Template_Parameter_With_Concept_133.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/Debug.h"

// ========================================================================================================
#define MUT133_OUTPUT 1

void MutatorFrontendAction_133::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !SourceMgr->isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *SourceMgr, 
          Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      //Replace template parameter with concept
      std::string declStr = declaration.str();
      size_t pos = declStr.find("typename");
      if (pos != std::string::npos) {
        declStr.replace(pos, 8, "concept");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), declStr);
    }
}

void MutatorFrontendAction_133::MutatorASTConsumer_133::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter, &Context, &Context.getSourceManager());
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}