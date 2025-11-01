//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Conflicting_Template_Constraints_85
 */ 
class MutatorFrontendAction_85 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(85)

private:
    class MutatorASTConsumer_85 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_85(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_85.h"

// ========================================================================================================
#define MUT85_OUTPUT 1

void MutatorFrontendAction_85::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplates")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getTemplateParameters()->size() != 1)
            return;

        auto *TP = FD->getTemplateParameters()->getParam(0);
        if (!TP || !TP->hasTypeConstraint())
            return;

        auto originalSource = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());

        std::string newConstraint = "OtherConceptName";
        std::string newFunctionTemplate = originalSource;
        size_t pos = newFunctionTemplate.find("requires");
        if (pos != std::string::npos) {
            newFunctionTemplate.insert(pos + 8, newConstraint + "<T> ");
        }

        newFunctionTemplate = "\n/*mut85*/" + newFunctionTemplate;
        Rewrite.InsertTextAfterToken(FD->getEndLoc(), newFunctionTemplate);
    }
}
  
void MutatorFrontendAction_85::MutatorASTConsumer_85::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(templateParameterList(sizeIs(1)), hasTypeConstraint())).bind("FunctionTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}