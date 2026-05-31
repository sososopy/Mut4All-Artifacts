//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"

/**
 * Insert_Explicit_Specialization_Member_Function_Template_146
 */ 
class MutatorFrontendAction_146 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(146)
private:
    class MutatorASTConsumer_146 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_146(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<std::string> classNames;
        std::vector<clang::SourceLocation> insertionPoints;
    };
};

//source file
#include "../include/Insert_Explicit_Specialization_Member_Function_Template_146.h"

// ========================================================================================================
#define MUT146_OUTPUT 1

void MutatorFrontendAction_146::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("classTemplate")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(CT->getLocation()))
            return;
        
        // Find a member function template with non-type parameters
        for (const auto *MD : CT->getTemplatedDecl()->methods()) {
            if (MD->isTemplated() && MD->getTemplateParameterList(0)) {
                const auto *FT = MD->getTemplateParameterList(0);
                if (FT->size() > 0) {
                    const auto *Param = FT->getParam(0);
                    if (isa<NonTypeTemplateParmDecl>(Param)) {
                        // Get insertion point after the class template
                        SourceLocation endLoc = CT->getTemplatedDecl()->getEndLoc();
                        insertionPoints.push_back(endLoc);
                        
                        // Get class name
                        std::string className = CT->getNameAsString();
                        classNames.push_back(className);
                        
                        // Find a member variable to capture in lambda
                        std::string memberVar = "x";
                        for (const auto *FD : CT->getTemplatedDecl()->fields()) {
                            if (FD->getType()->isIntegralOrEnumerationType()) {
                                memberVar = FD->getNameAsString();
                                break;
                            }
                        }
                        
                        // Generate specialization code
                        std::string specCode = "template<> auto " + className + "::" + 
                                              MD->getNameAsString() + "<2>() {\n" +
                                              "    return [this](int x) { return x + this->" + 
                                              memberVar + "; };\n" +
                                              "}\n";
                        
                        // Insert specialization
                        Rewrite.InsertText(endLoc, "/*mut146*/\n" + specCode, true, true);
                        return;
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_146::MutatorASTConsumer_146::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(
        has(cxxMethodDecl(
            hasTemplateParameterList(
                hasParameter(0, nonTypeTemplateParmDecl())
            )
        ).bind("methodTemplate")
    ).bind("classTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}