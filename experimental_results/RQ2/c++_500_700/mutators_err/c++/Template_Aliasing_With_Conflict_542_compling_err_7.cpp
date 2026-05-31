//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * template_aliasing_with_conflict_542
 */ 
class MutatorFrontendAction_542 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(542)

private:
    class MutatorASTConsumer_542 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_542(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        const TypeAliasDecl *OriginalAlias = nullptr;
    };
};

//source file
#include "../include/template_aliasing_with_conflict_542.h"

// ========================================================================================================
#define MUT542_OUTPUT 1

void MutatorFrontendAction_542::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TAD = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("typeAlias")) {
      //Filter nodes in header files
      if (!TAD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TAD->getLocation()))
        return;

      // Get the source code text of target node
      if (OriginalAlias == nullptr) {
        OriginalAlias = TAD;
        return;
      }

      // Perform mutation on the source code text by applying string replacement
      std::string aliasName = OriginalAlias->getNameAsString();
      std::string conflictingAlias = "ConflictType";
      std::string mutation = "template <typename T>\nusing " + conflictingAlias + " = int;\n";

      // Replace the original AST node with the mutated one
      auto aliasRange = OriginalAlias->getSourceRange();
      std::string originalText = std::string(Lexer::getSourceText(CharSourceRange::getTokenRange(aliasRange), *Result.SourceManager, Result.Context->getLangOpts()));
      std::string mutatedText = mutation + originalText;
      mutatedText.replace(mutatedText.find(aliasName), aliasName.length(), conflictingAlias);

      Rewrite.ReplaceText(aliasRange, mutatedText);
    }
}
  
void MutatorFrontendAction_542::MutatorASTConsumer_542::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasDecl(hasType(namedDecl(matchesName("std::variant")))).bind("typeAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}