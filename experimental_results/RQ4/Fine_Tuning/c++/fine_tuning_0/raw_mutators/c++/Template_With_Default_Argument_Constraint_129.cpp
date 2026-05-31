//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_With_Default_Argument_Constraint_129
 */ 
class MutatorFrontendAction_129 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(129)

private:
    class MutatorASTConsumer_129 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_129(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_With_Default_Argument_Constraint_129.h"

// ========================================================================================================
#define MUT129_OUTPUT 1

void MutatorFrontendAction_129::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      if (declaration.find('=') == string::npos)
        return;
      auto def = declaration.substr(declaration.find('='));
      if (def.find("requires") == string::npos)
        return;
      llvm::outs() << def << '\n';
      auto def_with_concept = def.substr(def.find("requires"));
      def_with_concept = "/*mut129*/" + def_with_concept;
      llvm::outs() << def_with_concept << '\n';
      if (def_with_concept.find(';') != string::npos)
        def_with_concept.erase(def_with_concept.find(';'));
      llvm::outs() << def_with_concept << '\n';
      def_with_concept = def_with_concept + " && false";
      declaration.replace(declaration.find(def_with_concept),
                          def_with_concept.length(), def_with_concept);
      llvm::outs() << declaration << '\n';
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_129::MutatorASTConsumer_129::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}