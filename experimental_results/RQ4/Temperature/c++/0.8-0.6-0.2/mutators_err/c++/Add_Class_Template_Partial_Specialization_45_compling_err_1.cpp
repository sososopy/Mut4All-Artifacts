//header file
#pragma once
#include "Mutator_base.h"

/**
 * add_class_template_partial_specialization_45
 */ 
class MutatorFrontendAction_45 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(45)

private:
    class MutatorASTConsumer_45 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_45(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT45_OUTPUT 1

void MutatorFrontendAction_45::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(CTD->getLocation()))
        return;

      if (CTD->getTemplateParameters()->size() != 1)
        return;

      std::string templateName = CTD->getNameAsString();
      std::string specialization = "template<typename T> class " + templateName + "<T&> {};";

      SourceLocation endLoc = CTD->getEndLoc();
      Rewrite.InsertTextAfterToken(endLoc, "\n/*mut45*/" + specialization);
    }
}

void MutatorFrontendAction_45::MutatorASTConsumer_45::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}