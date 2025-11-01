//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Function_Overloads_With_Constraints_455
 */ 
class MutatorFrontendAction_455 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(455)

private:
    class MutatorASTConsumer_455 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_455(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> overloadedFunctions;
    };
};

//source file
#include "../include/modify_function_overloads_with_constraints_455.h"

// ========================================================================================================
#define MUT455_OUTPUT 1

void MutatorFrontendAction_455::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
            return;

        if (MT->getParent()->isClass() && MT->getParent()->isTemplated()) {
            auto methodName = MT->getNameAsString();
            if (overloadedFunctions.find(methodName) == overloadedFunctions.end()) {
                overloadedFunctions.insert(methodName);

                std::string newOverload = "void " + methodName + "(double) { }";
                SourceLocation insertLoc = MT->getEndLoc().getLocWithOffset(1);
                Rewrite.InsertText(insertLoc, "\n/*mut455*/" + newOverload);
            }
        }
    }
}

void MutatorFrontendAction_455::MutatorASTConsumer_455::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isOverloaded(), ofClass(cxxRecordDecl(isTemplateInstantiation()))).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}