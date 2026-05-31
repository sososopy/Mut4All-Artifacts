//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Static_Assert_Message_With_User_Type_Instance_377
 */ 
class MutatorFrontendAction_377 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(377)
private:
    class MutatorASTConsumer_377 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_377(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl*> candidate_types;
    };
};

//source file
#include "../include/Replace_Static_Assert_Message_With_User_Type_Instance_377.h"

// ========================================================================================================
#define MUT377_OUTPUT 1

void MutatorFrontendAction_377::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *sa = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("staticAssert")) {
        if (!sa || !Result.Context->getSourceManager().isWrittenInMainFile(sa->getLocation()))
            return;

        if (candidate_types.empty())
            return;

        auto *messageExpr = Result.Nodes.getNodeAs<clang::StringLiteral>("messageExpr");
        if (!messageExpr)
            return;

        SourceLocation start = messageExpr->getBeginLoc();
        SourceLocation end = messageExpr->getEndLoc();

        int index = getrandom::getRandomIndex(candidate_types.size());
        const clang::CXXRecordDecl *T = candidate_types[index];
        std::string replacement = T->getNameAsString() + "{}";

        Rewrite.ReplaceText(SourceRange(start, end), replacement);
    } else if (auto *cd = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("candidateType")) {
        if (!cd || !Result.Context->getSourceManager().isWrittenInMainFile(cd->getLocation()))
            return;

        candidate_types.push_back(cd);
    }
}
  
void MutatorFrontendAction_377::MutatorASTConsumer_377::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher_static_assert = 
        staticAssertDecl(
            hasMessage(ignoringImplicit(stringLiteral().bind("messageExpr")))
        ).bind("staticAssert");

    DeclarationMatcher matcher_candidate_type = 
        cxxRecordDecl(
            hasMethod(cxxMethodDecl(
                hasName("size"),
                isConst(),
                returns(isInteger()),
                hasBody(compoundStmt()),
                hasAttr(attr::Constexpr)
            )),
            hasMethod(cxxMethodDecl(
                hasName("data"),
                isConst(),
                hasBody(compoundStmt())
            ))
        ).bind("candidateType");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher_static_assert, &callback);
    matchFinder.addMatcher(matcher_candidate_type, &callback);
    matchFinder.matchAST(Context);
}