//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Method_With_Dependent_Base_274
 */ 
class MutatorFrontendAction_274 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(274)

private:
    class MutatorASTConsumer_274 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_274(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_method_with_dependent_base_274.h"

// ========================================================================================================
#define MUT274_OUTPUT 1

void MutatorFrontendAction_274::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("TemplateMethod")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->hasBody()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string baseClass = "class Base { public: bool IsValid() const { return true; } };";
        std::string methodPointer = "return (this->*static_cast<bool (Base::*)()const>(&Base::IsValid))();";
        declaration.insert(declaration.find("{") + 1, "\n/*mut274*/" + methodPointer + "\n");
        declaration = baseClass + "\n" + declaration;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_274::MutatorASTConsumer_274::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(ofClass(cxxRecordDecl(isTemplateInstantiation()))).bind("TemplateMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}