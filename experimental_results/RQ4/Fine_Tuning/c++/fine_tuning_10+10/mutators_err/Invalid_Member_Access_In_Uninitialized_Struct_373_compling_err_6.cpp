//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Member_Access_In_Uninitialized_Struct_373
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
#include "../include/Mutator_373.h"

// ========================================================================================================
#define MUT373_OUTPUT 1

void MutatorFrontendAction_373::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        for (auto param : FD->parameters()) {
            if (auto *PT = param->getType()->getAs<clang::PointerType>()) {
                if (auto *RT = PT->getPointeeType()->getAs<clang::RecordType>()) {
                    if (RT->getDecl()->isCompleteDefinition()) {
                        auto funcBody = FD->getBody();
                        if (funcBody) {
                            std::string memberAccess = "int x = " + param->getNameAsString() + "->nonExistentMember;";
                            auto loc = funcBody->getBeginLoc().getLocWithOffset(1);
                            Rewrite.InsertText(loc, "/*mut373*/" + memberAccess + "\n");
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_373::MutatorASTConsumer_373::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasDescendant(parmVarDecl(hasType(pointerType(pointee(recordType(hasDeclaration(recordDecl()))))))).bind("funcDecl"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}