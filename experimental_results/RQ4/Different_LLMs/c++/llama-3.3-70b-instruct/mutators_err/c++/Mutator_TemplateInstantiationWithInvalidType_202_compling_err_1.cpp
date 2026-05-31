//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateInstantiationWithInvalidType_202
 */ 
class MutatorFrontendAction_TemplateInstantiationWithInvalidType_202 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(TemplateInstantiationWithInvalidType_202)

private:
    class MutatorASTConsumer_TemplateInstantiationWithInvalidType_202 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_TemplateInstantiationWithInvalidType_202(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_TemplateInstantiationWithInvalidType_202.h"

// ========================================================================================================
#define MUTTEMPLATEINSTANTIATIONWITHINVALIDTYPE_202_OUTPUT 1

void MutatorFrontendAction_TemplateInstantiationWithInvalidType_202::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateInstantiation")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Replace the original type with an invalid type, such as "void"
      std::string invalidType = "void";
      size_t pos = declaration.find(">");
      if (pos != std::string::npos) {
        declaration.replace(pos - 1, 1, invalidType);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_TemplateInstantiationWithInvalidType_202::MutatorASTConsumer_TemplateInstantiationWithInvalidType_202::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}