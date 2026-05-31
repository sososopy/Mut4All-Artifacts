//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Complex_Concept_Constraint_134
 */ 
class MutatorFrontendAction_134 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(134)

private:
    class MutatorASTConsumer_134 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_134(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_complex_concept_constraint_134.h"

// ========================================================================================================
#define MUT134_OUTPUT 1

void MutatorFrontendAction_134::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithConcept")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getType()->isFunctionProtoType()) {
            const auto *ProtoType = FD->getType()->getAs<clang::FunctionProtoType>();
            if (ProtoType->getRequiresClause()) {
                auto declaration = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
                std::string newConstraint = " && requires () { decltype(param)(); }";
                size_t pos = declaration.find("requires");
                if (pos != std::string::npos) {
                    pos = declaration.find("{", pos);
                    if (pos != std::string::npos) {
                        declaration.insert(pos, newConstraint);
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_134::MutatorASTConsumer_134::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasType(functionProtoType(
        hasRequiresClause()))).bind("FunctionWithConcept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}