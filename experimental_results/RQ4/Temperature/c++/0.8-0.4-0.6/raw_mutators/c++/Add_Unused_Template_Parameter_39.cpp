//header file
#pragma once
#include "Mutator_base.h"

/**
 * add_unused_template_parameter_39
 */ 
class MutatorFrontendAction_39 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(39)

private:
    class MutatorASTConsumer_39 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_39(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/add_unused_template_parameter_39.h"

// ========================================================================================================
#define MUT39_OUTPUT 1

void MutatorFrontendAction_39::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        auto *ClassDecl = dyn_cast<CXXRecordDecl>(FD->getDeclContext());
        if (!ClassDecl || !ClassDecl->isInNamespace())
            return;

        auto *TemplateParams = FD->getTemplateParameters();
        if (!TemplateParams || TemplateParams->size() == 0)
            return;

        auto FunctionSource = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
        std::string newTemplateParam = "typename Unused = int";
        std::string newUsage = "static_cast<Unused>(0);";

        size_t insertPos = FunctionSource.find(">");
        if (insertPos != std::string::npos) {
            FunctionSource.insert(insertPos, ", " + newTemplateParam);
        }

        size_t bodyPos = FunctionSource.find("{");
        if (bodyPos != std::string::npos) {
            FunctionSource.insert(bodyPos + 1, "\n/*mut39*/" + newUsage + "\n");
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), FunctionSource);
    }
}

void MutatorFrontendAction_39::MutatorASTConsumer_39::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasAncestor(namespaceDecl()), hasAncestor(cxxRecordDecl())).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}