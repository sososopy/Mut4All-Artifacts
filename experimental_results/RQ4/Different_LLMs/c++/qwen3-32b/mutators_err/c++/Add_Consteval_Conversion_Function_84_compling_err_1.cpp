//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Consteval_Conversion_Function_84
 */ 
class MutatorFrontendAction_84 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(84)
private:
    class MutatorASTConsumer_84 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_84(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Consteval_Conversion_Function_84.h"

// ========================================================================================================
#define MUT84_OUTPUT 1

void MutatorFrontendAction_84::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TargetClass")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CL->getLocation()))
            return;
        if (!CL->isCompleteDefinition())
            return;

        int choice = getrandom::getRandomIndex(3);
        std::string type, value;
        if (choice == 0) {
            type = "int";
            value = "0";
        } else if (choice == 1) {
            type = "bool";
            value = "true";
        } else {
            type = "float";
            value = "0.0f";
        }

        std::string conversionCode = "  consteval operator " + type + "() { return " + value + "; }\n";
        auto sourceCode = stringutils::rangetoStr(*Result.SourceManager, CL->getSourceRange());
        size_t pos = sourceCode.rfind('}');
        if (pos != std::string::npos) {
            sourceCode.insert(pos, conversionCode);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(CL->getSourceRange()), sourceCode);
        }
    }
}
  
void MutatorFrontendAction_84::MutatorASTConsumer_84::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(
        isStructOrClass(),
        hasMethod(cxxConstructorDecl(isConstEval())),
        unless(hasMethod(cxxConversionDecl()))
    ).bind("TargetClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}