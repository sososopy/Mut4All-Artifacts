//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Specialization_500
 */ 
class MutatorFrontendAction_500 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(500)

private:
    class MutatorASTConsumer_500 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_500(Rewriter &R) : TheRewriter(R) {}
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
        const FunctionDecl *SpecializedFunction = nullptr;
    };
};

//source file
#include "../include/Modify_Template_Specialization_500.h"

// ========================================================================================================
#define MUT500_OUTPUT 1

void MutatorFrontendAction_500::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("SpecializedFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      // Check if the function is a template specialization
      if (FD->isFunctionTemplateSpecialization()) {
        // Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
        
        // Perform mutation on the source code text by applying string replacement
        // Modify the return type to a compatible type, e.g., from int to double
        std::string newReturnType = "double";
        size_t pos = declaration.find("int");
        if (pos != std::string::npos) {
          declaration.replace(pos, 3, newReturnType);
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_500::MutatorASTConsumer_500::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplateSpecialization()).bind("SpecializedFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}