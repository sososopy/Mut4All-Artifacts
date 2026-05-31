//header file
#pragma once
#include "Mutator_base.h"

/**
 * Defaulted_Assignment_With_Explicit_Object_Parameter_351
 */ 
class MutatorFrontendAction_351 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(351)

private:
    class MutatorASTConsumer_351 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_351(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> derivedClasses;
    };
};

//source file
#include "../include/defaulted_assignment_with_explicit_object_parameter_351.h"

// ========================================================================================================
#define MUT351_OUTPUT 1

void MutatorFrontendAction_351::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Derived = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
        if (!Derived || !Result.Context->getSourceManager().isWrittenInMainFile(Derived->getLocation()))
            return;
        
        if (!Derived->isCompleteDefinition() || Derived->field_empty())
            return;

        auto baseSpecifier = Derived->bases_begin();
        if (baseSpecifier == Derived->bases_end())
            return;

        const clang::CXXRecordDecl *Base = baseSpecifier->getType()->getAsCXXRecordDecl();
        if (!Base || Base->isLambda())
            return;

        derivedClasses.push_back(Derived);

        std::string derivedName = Derived->getNameAsString();
        std::string baseName = Base->getNameAsString();

        std::string assignmentOperator = "\n/*mut351*/" + derivedName + "& operator=(this " + baseName + "& self, const " + derivedName + "&) = default;\n";
        Rewrite.InsertTextAfterToken(Derived->getBraceRange().getEnd(), assignmentOperator);
    }
}

void MutatorFrontendAction_351::MutatorASTConsumer_351::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(hasDefinition(), hasAnyBase(cxxRecordDecl(hasDefinition()).bind("Base"))).bind("Derived");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}