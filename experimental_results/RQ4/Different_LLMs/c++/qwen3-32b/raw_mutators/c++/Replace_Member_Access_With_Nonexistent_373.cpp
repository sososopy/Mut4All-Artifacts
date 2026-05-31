//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Member_Access_With_Nonexistent_373
 */ 
class MutatorFrontendAction_373 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(373)

private:
    class MutatorASTConsumer_373 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_373(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Member_Access_With_Nonexistent_373.h"

// ========================================================================================================
#define MUT373_OUTPUT 1

void MutatorFrontendAction_373::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ME = Result.Nodes.getNodeAs<clang::MemberExpr>("memberExpr")) {
        // Filter nodes in header files
        if (!ME || !Result.Context->getSourceManager().isWrittenInMainFile(ME->getBeginLoc()))
            return;

        // Check if the base type is a forward-declared struct
        QualType baseType = ME->getBase()->getType()->getPointeeType();
        if (!baseType->isIncompleteType())
            return;

        // Get the member name
        DeclarationNameInfo nameInfo = ME->getMemberNameInfo();
        if (!nameInfo.getName().isIdentifier())
            return;

        // Generate a new name
        std::string newName = "nonexistent_member";

        // Replace the name in the source
        SourceLocation nameLoc = nameInfo.getLocStart();
        SourceLocation endLoc = nameLoc.getLocWithOffset(ME->getMemberName().getLength() - 1);
        Rewrite.ReplaceText(SourceRange(nameLoc, endLoc), newName);
    }
}

void MutatorFrontendAction_373::MutatorASTConsumer_373::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define the AST matcher
    DeclarationMatcher matcher = memberExpr(
        hasParent(initListExpr()),
        hasObjectExpression(declRefExpr(to(varDecl(hasType(pointerType(pointee(recordType(isIncomplete()))))))))
    ).bind("memberExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}