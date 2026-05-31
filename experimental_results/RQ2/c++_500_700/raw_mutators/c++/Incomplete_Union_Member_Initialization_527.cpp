//header file
#pragma once
#include "Mutator_base.h"

/**
 * incomplete_union_member_initialization_527
 */ 
class MutatorFrontendAction_527 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(527)

private:
    class MutatorASTConsumer_527 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_527(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_527.h"

// ========================================================================================================
#define MUT527_OUTPUT 1

void MutatorFrontendAction_527::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::FieldDecl>("UnionMember")) {
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;

      if (UD->getInClassInitializer() == nullptr) {
        SourceLocation loc = UD->getEndLoc();
        Rewrite.InsertTextAfterToken(loc, " = /*mut527*/;");
      }
    }
}
  
void MutatorFrontendAction_527::MutatorASTConsumer_527::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl(hasParent(recordDecl(isUnion()))).bind("UnionMember");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}