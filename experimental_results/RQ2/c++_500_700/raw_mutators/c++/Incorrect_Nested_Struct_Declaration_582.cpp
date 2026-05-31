//header file
#pragma once
#include "Mutator_base.h"

/**
 * incorrect_nested_struct_declaration_582
 */ 
class MutatorFrontendAction_582 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(582)

private:
    class MutatorASTConsumer_582 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_582(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/incorrect_nested_struct_declaration_582.h"

// ========================================================================================================
#define MUT582_OUTPUT 1

void MutatorFrontendAction_582::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
        if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(SD->getLocation()))
            return;

        if (SD->isStruct() && SD->isCompleteDefinition()) {
            auto structName = SD->getNameAsString();
            std::string incorrectNestedDecl = "struct " + structName + ";";
            SourceLocation insertLoc = SD->getLocation().getLocWithOffset(1);
            Rewrite.InsertTextAfterToken(insertLoc, "\n/*mut582*/" + incorrectNestedDecl);
        }
    }
}

void MutatorFrontendAction_582::MutatorASTConsumer_582::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}