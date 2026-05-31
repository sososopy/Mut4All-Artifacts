//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Lambda_Capture_Struct_Field_345
 */ 
class MutatorFrontendAction_345 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(345)

private:
    class MutatorASTConsumer_345 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_345(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Lambda_Capture_Struct_Field_345.h"

// ========================================================================================================
#define MUT345_OUTPUT 1

void MutatorFrontendAction_345::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("field")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        const auto *RD = FD->getDeclContext()->getRecordDecl();
        if (!RD || !RD->isCompleteDefinition())
            return;

        const FieldDecl *OtherFD = nullptr;
        for (const auto *Candidate : RD->fields()) {
            if (Candidate != FD) {
                OtherFD = Candidate;
                break;
            }
        }
        if (!OtherFD)
            return;

        std::string OtherFieldName = OtherFD->getNameAsString();
        SourceLocation EqualLoc = Rewrite.getLocForEndOfToken(FD->getLocation());
        if (EqualLoc.isInvalid())
            return;

        Expr *Init = FD->getInClassInitializer();
        if (!Init)
            return;
        SourceLocation InitEndLoc = Init->getEndLoc();

        std::string LambdaCode = "[this]() { return this->" + OtherFieldName + "; }()";
        Rewrite.ReplaceText(SourceRange(EqualLoc, InitEndLoc), "=" + LambdaCode);
    }
}

void MutatorFrontendAction_345::MutatorASTConsumer_345::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl(hasInClassInitializer(anything())).bind("field");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}