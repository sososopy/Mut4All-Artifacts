//header file
#pragma once
#include "Mutator_base.h"

/**
 * invalid_using_declaration_in_derived_class_654
 */ 
class MutatorFrontendAction_654 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(654)

private:
    class MutatorASTConsumer_654 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_654(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> base_classes;
    };
};

//source file
#include "../include/invalid_using_declaration_in_derived_class_654.h"

// ========================================================================================================
#define MUT654_OUTPUT 1

void MutatorFrontendAction_654::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Base = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Base")) {
        if (!Base || !Result.Context->getSourceManager().isWrittenInMainFile(Base->getLocation()))
            return;
        if (Base->isCompleteDefinition() && Base->hasDefinition()) {
            base_classes.push_back(Base);
        }
    } else if (auto *Derived = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
        if (!Derived || !Result.Context->getSourceManager().isWrittenInMainFile(Derived->getLocation()))
            return;
        if (Derived->isCompleteDefinition()) {
            for (const auto *Base : base_classes) {
                if (Derived->isDerivedFrom(Base)) {
                    std::string invalidUsing = "\npublic:\nusing " + Base->getNameAsString() + " :: ;\n";
                    invalidUsing = "/*mut654*/" + invalidUsing;
                    Rewrite.InsertTextAfterToken(Derived->getEndLoc(), invalidUsing);
                }
            }
        }
    }
}

void MutatorFrontendAction_654::MutatorASTConsumer_654::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl(isDefinition(), anyOf(isClass(), isStruct())).bind("Base");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}