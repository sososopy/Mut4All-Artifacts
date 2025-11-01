//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Function_Argument_Overload_49
 */ 
class MutatorFrontendAction_49 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(49)

private:
    class MutatorASTConsumer_49 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_49(Rewriter &R) : TheRewriter(R) {}
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
        const CXXRecordDecl *currentClass = nullptr;
        const FunctionTemplateDecl *originalTemplate = nullptr;
    };
};

//source file
#include "../include/template_function_argument_overload_49.h"

// ========================================================================================================
#define MUT49_OUTPUT 1

void MutatorFrontendAction_49::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FTD->getLocation()))
            return;

        if (auto *CRD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Class")) {
            if (!CRD->isCompleteDefinition())
                return;

            currentClass = CRD;
            originalTemplate = FTD;

            //Get the source code text of target node
            std::string originalFunction = stringutils::rangetoStr(*(Result.SourceManager), FTD->getSourceRange());

            //Perform mutation on the source code text by applying string replacement
            std::string overloadedFunction = originalFunction;
            size_t pos = overloadedFunction.find(")");
            if (pos != std::string::npos) {
                overloadedFunction.insert(pos, ", int extra");
            }

            std::string callSite = "/*mut49*/static constexpr int testValue = ";
            callSite += originalTemplate->getNameAsString() + "<int>(42);";

            //Replace the original AST node with the mutated one
            std::string mutation = originalFunction + "\n" + overloadedFunction + "\n" + callSite;
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(CRD->getSourceRange()), mutation);
        }
    }
}
  
void MutatorFrontendAction_49::MutatorASTConsumer_49::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasParent(cxxRecordDecl().bind("Class"))).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}