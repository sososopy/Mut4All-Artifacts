//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Operator_Parameter_Type_144
 */ 
class MutatorFrontendAction_144 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(144)

private:
    class MutatorASTConsumer_144 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_144(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutate_Operator_Parameter_Type_144.h"

// ========================================================================================================
#define MUT144_OUTPUT 1

void MutatorFrontendAction_144::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MethodDecl = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("operatorOverload")) {
        if (!MethodDecl || !Result.Context->getSourceManager().isWrittenInMainFile(MethodDecl->getLocation()))
            return;

        if (MethodDecl->getNumParams() > 0) {
            auto *FirstParam = MethodDecl->getParamDecl(0);
            if (FirstParam) {
                QualType OriginalType = FirstParam->getType();
                std::string OriginalTypeName = OriginalType.getAsString();

                // Perform mutation: change the type of the first parameter to an unrelated class type
                std::string NewTypeName = "UnrelatedClass";
                std::string MutatedParamDecl = NewTypeName + " " + FirstParam->getNameAsString();
                
                // Get the source range of the parameter to replace
                SourceRange ParamRange = FirstParam->getSourceRange();
                Rewrite.ReplaceText(ParamRange, MutatedParamDecl);
            }
        }
    }
}
  
void MutatorFrontendAction_144::MutatorASTConsumer_144::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isOverloadedOperatorCall(), hasName("operator delete")).bind("operatorOverload");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}