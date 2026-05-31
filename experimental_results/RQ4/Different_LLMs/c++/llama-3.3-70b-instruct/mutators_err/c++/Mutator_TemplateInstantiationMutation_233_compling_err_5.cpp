//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * TemplateInstantiationMutation_233
 */ 
class MutatorFrontendAction_TemplateInstantiationMutation_233 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_TemplateInstantiationMutation_233>(TheRewriter);
    }

private:
    class MutatorASTConsumer_TemplateInstantiationMutation_233 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_TemplateInstantiationMutation_233(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateDecl *> templateDecls;
    };
};

//source file
#include "../include/Mutator_TemplateInstantiationMutation_233.h"
#include "clang/ASTMatchers/ASTMatchers.h"

void MutatorFrontendAction_TemplateInstantiationMutation_233::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   TD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace template arguments with other types already present in the code
      // or introduce new template arguments that are derived from the existing types
      std::string mutatedDeclaration = declaration;
      // For example, replace 'int' with 'double' in template arguments
      size_t pos = mutatedDeclaration.find("int");
      if (pos != std::string::npos) {
        mutatedDeclaration.replace(pos, 3, "double");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), mutatedDeclaration);
    }
    else if (auto *TI = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateInstantiation")) {
      //Filter nodes in header files
      if (!TI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TI->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   TI->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace template arguments with other types already present in the code
      // or introduce new template arguments that are derived from the existing types
      std::string mutatedDeclaration = declaration;
      // For example, replace 'int' with 'double' in template arguments
      size_t pos = mutatedDeclaration.find("int");
      if (pos != std::string::npos) {
        mutatedDeclaration.replace(pos, 3, "double");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TI->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_TemplateInstantiationMutation_233::MutatorASTConsumer_TemplateInstantiationMutation_233::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = cxxRecordDecl().bind("TemplateDecl");
    DeclarationMatcher matcher2 = classTemplateSpecializationDecl().bind("TemplateInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}