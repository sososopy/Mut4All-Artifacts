//header file
#pragma once
#include "Mutator_base.h"

/**
 * Duplicate_Destructor_Overload_Resolution_172
 */ 
class MutatorFrontendAction_172 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(172)

private:
    class MutatorASTConsumer_172 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_172(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Duplicate_Destructor_Overload_Resolution_172.h"

// ========================================================================================================
#define MUT172_OUTPUT 1

void MutatorFrontendAction_172::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassWithDtor")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
            return;
        if (!CL->isCompleteDefinition())
            return;

        auto className = CL->getNameAsString();
        if (className.empty())
            return;

        std::string newDtor = "~" + className + "() {}";

        auto content = stringutils::rangetoStr(*Result.SourceManager, CL->getSourceRange());
        size_t pos = content.rfind('}');
        if (pos == std::string::npos)
            return;

        content.insert(pos, "\n    " + newDtor);
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CL->getSourceRange()), content);
    }
}

void MutatorFrontendAction_172::MutatorASTConsumer_172::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(has(cxxDestructorDecl(isUserProvided()))).bind("ClassWithDtor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}