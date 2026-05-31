//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Explicit_Template_Argument_With_Dependent_Type_62
 */ 
class MutatorFrontendAction_62 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(62)

private:
    class MutatorASTConsumer_62 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_62(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::vector<const clang::DeclContext *> templateContexts;
        std::vector<std::string> templateParamNames;
    };
};

//source file
#include "../include/Mutator_Replace_Explicit_Template_Argument_With_Dependent_Type_62.h"

// ========================================================================================================
#define MUT62_OUTPUT 1

void MutatorFrontendAction_62::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunction")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        
        // Record template contexts and parameter names for later use
        if (FD->getDescribedFunctionTemplate()) {
            templateContexts.push_back(FD->getDeclContext());
            auto *TemplateParams = FD->getDescribedFunctionTemplate()->getTemplateParameters();
            for (unsigned i = 0; i < TemplateParams->size(); ++i) {
                if (auto *TTP = dyn_cast<clang::TemplateTypeParmDecl>(TemplateParams->getParam(i))) {
                    templateParamNames.push_back(TTP->getNameAsString());
                }
            }
        }
    }
    else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass")) {
        //Filter nodes in header files
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;
        
        // Record template class contexts and parameter names
        if (CD->getDescribedClassTemplate()) {
            templateContexts.push_back(CD->getDeclContext());
            auto *TemplateParams = CD->getDescribedClassTemplate()->getTemplateParameters();
            for (unsigned i = 0; i < TemplateParams->size(); ++i) {
                if (auto *TTP = dyn_cast<clang::TemplateTypeParmDecl>(TemplateParams->getParam(i))) {
                    templateParamNames.push_back(TTP->getNameAsString());
                }
            }
        }
    }
    else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("ExplicitTemplateCall")) {
        //Filter nodes in header files
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;
        
        // Check if this is a call to a function template with explicit template arguments
        auto *DRE = dyn_cast<clang::DeclRefExpr>(CE->getCallee()->IgnoreImpCasts());
        if (!DRE) return;
        
        auto *FD = dyn_cast<clang::FunctionDecl>(DRE->getDecl());
        if (!FD || !FD->getPrimaryTemplate()) return;
        
        // Get the explicit template arguments
        auto *TemplateArgs = CE->getExplicitTemplateArgs();
        if (!TemplateArgs || TemplateArgs->size() == 0) return;
        
        // Find a suitable template parameter from enclosing context
        std::string replacementParam;
        for (const auto& param : templateParamNames) {
            if (!param.empty()) {
                replacementParam = param;
                break;
            }
        }
        
        if (replacementParam.empty()) {
            // No existing template parameter found, create a new one
            replacementParam = "U";
        }
        
        // Choose a random template argument to replace
        unsigned argIndex = getrandom::getRandomIndex(TemplateArgs->size() - 1);
        auto *ArgLoc = TemplateArgs->getTemplateArgLoc(argIndex);
        
        // Get the source range of the template argument
        SourceRange ArgRange = ArgLoc.getSourceRange();
        if (ArgRange.isInvalid()) return;
        
        // Perform mutation: replace with dependent type
        std::string replacement = replacementParam;
        // 50% chance to make it a pointer or reference for more complexity
        if (getrandom::getRandomIndex(1) == 0) {
            if (getrandom::getRandomIndex(1) == 0) {
                replacement += "*";
            } else {
                replacement += "&";
            }
        }
        
        // Replace the original template argument with dependent type
        Rewrite.ReplaceText(ArgRange, replacement);
    }
}
  
void MutatorFrontendAction_62::MutatorASTConsumer_62::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify template functions and classes to collect context
    DeclarationMatcher templateFuncMatcher = functionDecl(isTemplateInstantiation()).bind("TemplateFunction");
    DeclarationMatcher templateClassMatcher = cxxRecordDecl(isTemplateInstantiation()).bind("TemplateClass");
    //Define matcher for explicit template function calls
    StatementMatcher callMatcher = callExpr(hasExplicitTemplateArgs()).bind("ExplicitTemplateCall");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateFuncMatcher, &callback);
    matchFinder.addMatcher(templateClassMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}