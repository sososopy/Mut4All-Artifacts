//header file
#pragma once
#include "Mutator_base.h"

/**
 * Export_Template_Specialization_Without_Module_Context_66
 */ 
class MutatorFrontendAction_66 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(66)

private:
    class MutatorASTConsumer_66 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_66(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_66.h"

// ========================================================================================================
#define MUT66_OUTPUT 1

void MutatorFrontendAction_66::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarTemplateSpecializationDecl>("VarTemplateSpec")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if already has export
      if (VD->getTemplateSpecializationKind() == clang::TSK_ExplicitSpecialization)
        return;
      //Get the source code text of target node
      auto SourceRange = VD->getSourceRange();
      std::string OriginalText = stringutils::rangetoStr(*(Result.SourceManager), SourceRange);
      //Perform mutation on the source code text by applying string replacement
      std::string MutatedText = "export " + OriginalText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SourceRange), MutatedText);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionTemplateSpec")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if it's a template specialization and not already exported
      if (!FD->isFunctionTemplateSpecialization() || FD->getTemplateSpecializationKind() == clang::TSK_ExplicitSpecialization)
        return;
      //Get the source code text of target node
      auto SourceRange = FD->getSourceRange();
      std::string OriginalText = stringutils::rangetoStr(*(Result.SourceManager), SourceRange);
      //Perform mutation on the source code text by applying string replacement
      std::string MutatedText = "export " + OriginalText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SourceRange), MutatedText);
    }
    else if (auto *CD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("ClassTemplateSpec")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      //Check if not already exported
      if (CD->getTemplateSpecializationKind() == clang::TSK_ExplicitSpecialization)
         return;
      //Get the source code text of target node
      auto SourceRange = CD->getSourceRange();
      std::string OriginalText = stringutils::rangetoStr(*(Result.SourceManager), SourceRange);
      //Perform mutation on the source code text by applying string replacement
      std::string MutatedText = "export " + OriginalText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SourceRange), MutatedText);
    }
}
  
void MutatorFrontendAction_66::MutatorASTConsumer_66::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = varTemplateSpecializationDecl(hasParent(decl().bind("VarTemplateSpec")));
    DeclarationMatcher matcher2 = functionDecl(isTemplateSpecialization()).bind("FunctionTemplateSpec");
    DeclarationMatcher matcher3 = classTemplateSpecializationDecl().bind("ClassTemplateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.addMatcher(matcher3, &callback);
    matchFinder.matchAST(Context);
}