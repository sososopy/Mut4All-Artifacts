//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alter_Transparent_Union_Declaration_34
 */ 
class MutatorFrontendAction_34 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(34)

private:
    class MutatorASTConsumer_34 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_34(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Alter_Transparent_Union_Declaration_34.h"

// ========================================================================================================
#define MUT34_OUTPUT 1

void MutatorFrontendAction_34::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UT = Result.Nodes.getNodeAs<clang::TypedefDecl>("TransparentUnion")) {
        if (!UT || !Result.Context->getSourceManager().isWrittenInMainFile(UT->getLocation()))
            return;

        if (const auto *UTD = dyn_cast<clang::ElaboratedType>(UT->getUnderlyingType().getTypePtr())) {
            if (const auto *UD = dyn_cast<clang::RecordType>(UTD->getNamedType().getTypePtr())) {
                if (const auto *RD = dyn_cast<clang::RecordDecl>(UD->getDecl())) {
                    if (RD->isUnion() && RD->hasAttr<clang::TransparentUnionAttr>()) {
                        std::string unionName = RD->getNameAsString();
                        std::string newUnionDecl = "typedef union { union " + unionName + " inner; int z; } " + unionName + " __attribute__((__transparent_union__));";
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(UT->getSourceRange()), newUnionDecl);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_34::MutatorASTConsumer_34::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typedefDecl(hasType(elaboratedType(hasNamedType(recordType(hasDeclaration(recordDecl(isUnion(), hasAttr<TransparentUnionAttr>()))))))).bind("TransparentUnion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}