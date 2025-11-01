//header file
#pragma once
#include "Mutator_base.h"

/**
 * Overload_Function_Template_Specialization_304
 */ 
class MutatorFrontendAction_304 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(304)

private:
    class MutatorASTConsumer_304 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_304(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Overload_Function_Template_Specialization_304.h"

// ========================================================================================================
#define MUT304_OUTPUT 1

void MutatorFrontendAction_304::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionTemplate")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isFunctionTemplateSpecialization()) {
        const TemplateArgumentList *TemplateArgs = FD->getTemplateSpecializationArgs();
        if (!TemplateArgs)
          return;

        std::string newConceptName = "D";
        std::string newConceptDef = "template<typename T>\nconcept D = requires(T t) { g(t); };\n";
        std::string newFunctionDef = "auto " + FD->getNameAsString() + "(D auto);\n";

        SourceLocation insertLoc = FD->getEndLoc().getLocWithOffset(1);
        std::string insertionText = "\n/*mut304*/" + newConceptDef + newFunctionDef;
        Rewrite.InsertTextAfter(insertLoc, insertionText);
      }
    }
}
  
void MutatorFrontendAction_304::MutatorASTConsumer_304::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasAttr<clang::TemplateSpecializationAttr>()).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}