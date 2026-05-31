//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Instantiation_455
 */ 
class MutatorFrontendAction_455 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(455)

private:
    class MutatorASTConsumer_455 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_455(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateSpecializationTypeLoc*> templateInstantiations;
    };
};

//source file
#include "../include/Replace_Template_Instantiation_455.h"

// ========================================================================================================
#define MUT455_OUTPUT 1

void MutatorFrontendAction_455::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TI = Result.Nodes.getNodeAs<clang::TemplateSpecializationTypeLoc>("TemplateInstantiation")) {
      //Filter nodes in header files
      if (!TI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TI->getSourceRange().getBegin()))
        return;
      //Get the source code text of target node
      templateInstantiations.push_back(TI);
    }
    else if (auto *TU = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("TranslationUnit")) {
      //Filter nodes in header files
      if (!TU || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TU->getLocation()))
        return;
      //Perform mutation on the source code text by applying string replacement
      for (auto TI : templateInstantiations) {
        auto templateName = TI->getTemplateNameLoc().getTemplateName().getAsString();
        auto templateArgs = TI->getTemplateArgumentsAsWritten();
        auto templateArg = templateArgs.get(0);
        auto templateArgType = templateArg.getType();
        auto templateArgTypeStr = templateArgType.getAsString();
        // Replace the type with another type
        auto newType = "double"; // Replace with another type
        auto newTemplateArg = clang::TemplateArgument(clang::QualType(clang::Type::getFromPtrInfo(Result.Context, newType)));
        auto newTemplateArgs = clang::TemplateArgumentList({newTemplateArg});
        auto newTemplateInstantiation = clang::TemplateSpecializationTypeLoc(newTemplateArgs, TI->getTemplateNameLoc().getTemplateName());
        auto newTemplateInstantiationStr = newTemplateInstantiation.getAsString();
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TI->getSourceRange()), newTemplateInstantiationStr);
      }
    }
}
  
void MutatorFrontendAction_455::MutatorASTConsumer_455::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationTypeLoc().bind("TemplateInstantiation");
    DeclarationMatcher tuMatcher = translationUnitDecl().bind("TranslationUnit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(tuMatcher, &callback);
    matchFinder.matchAST(Context);
}