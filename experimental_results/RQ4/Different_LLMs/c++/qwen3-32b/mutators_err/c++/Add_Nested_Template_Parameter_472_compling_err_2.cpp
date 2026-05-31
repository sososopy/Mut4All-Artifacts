//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Nested_Template_Parameter_472
 */ 
class MutatorFrontendAction_472 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(472)

private:
    class MutatorASTConsumer_472 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_472(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Add_Nested_Template_Parameter_472.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/Basic/SourceLocation.h"

// ========================================================================================================
#define MUT472_OUTPUT 1

void MutatorFrontendAction_472::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        // Filter nodes in header files
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(FT->getLocation()))
            return;

        clang::DeclContext *DC = FT->getDeclContext();
        if (auto *ClassDecl = DC->getAsCXXRecordDecl()) {
            if (auto *ClassTemplate = ClassDecl->getDescribedClassTemplate()) {
                clang::TemplateParameterList *ClassTPL = ClassTemplate->getTemplateParameters();
                if (ClassTPL->size() < 1)
                    return;
                clang::TemplateParameter *TParam = ClassTPL->getParam(0);
                std::string TName = TParam->getNameAsString();

                clang::TemplateParameterList *FuncTPL = FT->getTemplateParameters();
                if (FuncTPL->size() < 1)
                    return;
                clang::TemplateParameter *UParam = FuncTPL->getParam(0);
                std::string UName = UParam->getNameAsString();

                // Get the source code of the function's template declaration
                SourceManager &SM = *Result.SourceManager;
                auto sourceRange = FT->getSourceRange();
                if (sourceRange.isInvalid())
                    return;

                std::string originalTemplate = stringutils::rangetoStr(SM, sourceRange);

                // Find the '>' to replace
                size_t pos = originalTemplate.find('>');
                if (pos == std::string::npos)
                    return;

                std::string newParams = ", typename V = std::vector<" + TName + ">, typename W = std::pair<V, " + UName + ">>";
                std::string newTemplate = originalTemplate.substr(0, pos) + newParams;

                // Apply the mutation
                Rewrite.ReplaceText(sourceRange, newTemplate);
            }
        }
    }
}

void MutatorFrontendAction_472::MutatorASTConsumer_472::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to identify function templates inside class templates
    DeclarationMatcher matcher = functionTemplateDecl(hasDeclContext(classTemplateDecl())).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}