//header file
#pragma once
#include "Mutator_base.h"

/**
 * Swap_Template_Specialization_191
 */ 
class MutatorFrontendAction_Swap_Template_Specialization_191 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_Swap_Template_Specialization_191>(TheRewriter);
    }

private:
    class MutatorASTConsumer_Swap_Template_Specialization_191 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Swap_Template_Specialization_191(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateSpecializationDecl*> specializations;
    };
};

//source file
#include "../include/Mutator_Swap_Template_Specialization_191.h"

// ========================================================================================================
#define MUT191_OUTPUT 1

void MutatorFrontendAction_Swap_Template_Specialization_191::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *spec = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("Specialization")) {
      //Filter nodes in header files
      if (!spec || !Result.Context->getSourceManager().isWrittenInMainFile(
                     spec->getLocation()))
        return;
      //Record the specialization for potential swapping
      specializations.push_back(spec);
    }
    else if (specializations.size() >= 2) {
      // Get the last two specializations
      auto spec1 = specializations[specializations.size() - 1];
      auto spec2 = specializations[specializations.size() - 2];
      
      // Get the template arguments of the specializations
      auto args1 = spec1->getTemplateArgs();
      auto args2 = spec2->getTemplateArgs();
      
      // Swap the template arguments
      for (unsigned i = 0; i < args1.size(); ++i) {
        auto arg1 = args1[i];
        auto arg2 = args2[i];
        // Perform mutation on the source code text by applying string replacement
        std::string spec1Str = stringutils::rangetoStr(*(Result.SourceManager), arg1.getSourceRange());
        std::string spec2Str = stringutils::rangetoStr(*(Result.SourceManager), arg2.getSourceRange());
        
        // Replace the original AST nodes with the mutated ones
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(arg1.getSourceRange()), spec2Str);
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(arg2.getSourceRange()), spec1Str);
      }
    }
}
  
void MutatorFrontendAction_Swap_Template_Specialization_191::MutatorASTConsumer_Swap_Template_Specialization_191::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("Specialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}