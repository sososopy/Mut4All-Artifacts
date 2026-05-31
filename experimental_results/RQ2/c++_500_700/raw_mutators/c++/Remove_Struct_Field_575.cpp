//header file
#pragma once
#include "Mutator_base.h"

/**
 * remove_struct_field_575
 */ 
class MutatorFrontendAction_575 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(575)

private:
    class MutatorASTConsumer_575 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_575(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FieldDecl *> fieldsToRemove;
    };
};

//source file
#include "../include/remove_struct_field_575.h"

// ========================================================================================================
#define MUT575_OUTPUT 1

void MutatorFrontendAction_575::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
        if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(SD->getLocation()))
            return;
        
        if (!SD->isStruct() || !SD->isCompleteDefinition())
            return;

        for (auto *field : SD->fields()) {
            if (field->getIdentifier() && !field->isReferenced()) {
                fieldsToRemove.push_back(field);
                break;
            }
        }

        for (auto *field : fieldsToRemove) {
            Rewrite.RemoveText(field->getSourceRange());
        }
    }
}

void MutatorFrontendAction_575::MutatorASTConsumer_575::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}