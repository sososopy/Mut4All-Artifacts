#pragma once
#include "Mutator_base.h"

/**
 * Replace_Non_Dependent_Call_245
 */ 
class MutatorFrontendAction_245 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(245)
private:
    class MutatorASTConsumer_245 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_245(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> templateClasses;
    };
};

#include "../include/Replace_Non_Dependent_Call_245.h"

// ========================================================================================================
#define MUT245_OUTPUT 1

void MutatorFrontendAction_245::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CRD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass")) {
        //Filter nodes in header files
        if (!CRD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CRD->getLocation()))
            return;
        if (!CRD->isTemplateDecl())
            return;
        templateClasses.push_back(CRD);
    }
    else if (auto *CE = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("MemberCall")) {
        //Filter nodes in header files
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
            return;
        
        // Check if this is a call to a conversion operator
        const clang::CXXMethodDecl *MD = CE->getMethodDecl();
        if (!MD || !MD->isConversionFunction())
            return;
        
        // Check if the call is within a member function of a template class
        const clang::DeclContext *DC = CE->getDeclContext();
        if (!DC)
            return;
        
        const clang::CXXRecordDecl *ParentClass = nullptr;
        for (const auto *Class : templateClasses) {
            if (Class->containsDecl(CE)) {
                ParentClass = Class;
                break;
            }
        }
        
        if (!ParentClass)
            return;
        
        // Get the source code text of target node
        std::string callText = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
        
        // Perform mutation on the source code text by applying string replacement
        // Replace the non-dependent call with a dependent one using the class template parameter
        const clang::ClassTemplateDecl *CTD = ParentClass->getDescribedClassTemplate();
        if (!CTD)
            return;
        
        const clang::TemplateParameterList *TPL = CTD->getTemplateParameters();
        if (TPL && TPL->size() > 0) {
            const clang::NamedDecl *Param = TPL->getParam(0);
            std::string paramName = Param->getNameAsString();
            
            // Replace the type in the conversion operator call with the template parameter
            std::string newCallText = "operator " + paramName + "()";
            
            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), newCallText);
        }
    }
}

void MutatorFrontendAction_245::MutatorASTConsumer_245::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateClassMatcher = cxxRecordDecl(isTemplateDecl()).bind("TemplateClass");
    StatementMatcher memberCallMatcher = cxxMemberCallExpr(hasDeclaration(cxxMethodDecl(isConversionFunction()))).bind("MemberCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateClassMatcher, &callback);
    matchFinder.addMatcher(memberCallMatcher, &callback);
    matchFinder.matchAST(Context);
}