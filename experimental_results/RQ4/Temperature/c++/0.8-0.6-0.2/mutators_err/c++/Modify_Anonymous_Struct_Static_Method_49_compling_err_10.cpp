//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * modify_anonymous_struct_static_method_49
 */ 
class MutatorFrontendAction_49 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(49)

private:
    class MutatorASTConsumer_49 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_49(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_anonymous_struct_static_method_49.h"

// ========================================================================================================
#define MUT49_OUTPUT 1

void MutatorFrontendAction_49::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("anonStruct")) {
      if (!Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
        return;

      if (RD->isAnonymousStructOrUnion() && RD->isCompleteDefinition()) {
        for (const auto *M : RD->methods()) {
          if (M->isStatic()) {
            const auto *methodBody = M->getBody();
            if (!methodBody || methodBody->child_begin() == methodBody->child_end()) {
              std::string invalidCode = "{ /*mut49*/ invalid_code";
              Rewrite.InsertTextAfterToken(methodBody->getEndLoc(), invalidCode);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_49::MutatorASTConsumer_49::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isAnonymousStructOrUnion()).bind("anonStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}