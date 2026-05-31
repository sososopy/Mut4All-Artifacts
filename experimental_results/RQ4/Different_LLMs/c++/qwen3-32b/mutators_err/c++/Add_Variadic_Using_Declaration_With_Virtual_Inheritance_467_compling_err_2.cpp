//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/Type.h"

/**
 * Add_Variadic_Using_Declaration_With_Virtual_Inheritance_467
 */ 
class MutatorFrontendAction_467 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(467)
private:
    class MutatorASTConsumer_467 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_467(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Add_Variadic_Using_Declaration_With_Virtual_Inheritance_467.h"

// ========================================================================================================
#define MUT467_OUTPUT 1

void MutatorFrontendAction_467::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TargetClass")) {
        // Filter nodes in header files
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;

        // Check if class template has parameter pack with virtual inheritance
        if (auto *Template = CL->getDescribedClassTemplate()) {
            for (const auto *Param : Template->getTemplateParameters()->asArray()) {
                if (auto *TTP = dyn_cast<clang::TemplateTypeParmDecl>(Param)) {
                    if (TTP->isParameterPack()) {
                        // Check if base uses pack with virtual inheritance
                        for (const auto &Base : CL->bases()) {
                            if (Base.isVirtual() && 
                                Base.getType()->getAs<clang::PackExpansionType>()) {
                                // Generate using declaration with fold expression
                                std::string packName = TTP->getNameAsString();
                                std::string usingDecl = "using " + packName + "::member...;\n";
                                usingDecl = "/*mut467*/" + usingDecl;
                                // Insert before closing brace
                                SourceLocation loc = CL->getEndLoc();
                                if (loc.isValid()) {
                                    Rewrite.InsertText(loc, usingDecl, true, true);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_467::MutatorASTConsumer_467::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify class templates with virtual inheritance from parameter pack
    DeclarationMatcher matcher = cxxRecordDecl(
        isTemplateInstantiation(), 
        hasAnyBaseSpecifier(clang::ast_matchers::baseSpecifier(isVirtual()))
    ).bind("TargetClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}