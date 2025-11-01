//header file
#pragma once
#include "Mutator_base.h"

/**
 * Null_Pointer_Dereference_In_Base_Class_Handling_190
 */ 
class MutatorFrontendAction_190 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(190)

private:
    class MutatorASTConsumer_190 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_190(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/null_pointer_dereference_in_base_class_handling_190.h"

// ========================================================================================================
#define MUT190_OUTPUT 1

void MutatorFrontendAction_190::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Base = Result.Nodes.getNodeAs<clang::CXXBaseSpecifier>("BaseSpecifier")) {
        if (!Base || !Result.Context->getSourceManager().isWrittenInMainFile(Base->getBeginLoc()))
            return;

        auto baseType = Base->getType();
        auto baseDecl = baseType->getAsCXXRecordDecl();
        if (!baseDecl)
            return;

        std::string mutation = "const CXXRecordDecl* baseDecl = nullptr;\n";
        mutation += "#ifdef MUT190_OUTPUT\n";
        mutation += "if (baseType->isDependentType()) baseDecl = nullptr;\n";
        mutation += "else baseDecl = baseType->getAsCXXRecordDecl();\n";
        mutation += "#endif\n";

        SourceLocation insertLoc = Base->getBeginLoc();
        Rewrite.InsertText(insertLoc, mutation, true, true);
    }
}

void MutatorFrontendAction_190::MutatorASTConsumer_190::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl(hasBaseSpecifier(cxxBaseSpecifier().bind("BaseSpecifier")));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}