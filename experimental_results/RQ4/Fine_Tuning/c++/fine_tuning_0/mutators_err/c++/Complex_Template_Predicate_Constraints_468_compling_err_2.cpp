//header file
#pragma once
#include "Mutator_base.h"

/**
 * Complex_Template_Predicate_Constraints_468
 */ 
class MutatorFrontendAction_468 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(468)

private:
    class MutatorASTConsumer_468 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_468(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Complex_Template_Predicate_Constraints_468.h"

// ========================================================================================================
#define MUT468_OUTPUT 1

void MutatorFrontendAction_468::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      llvm::outs() << declaration << '\n';
      if (declaration.find("template") != string::npos) {
        auto pos = declaration.find("template");
        declaration.insert(pos, "/*mut468*/");
      }
      if (declaration.find("std::predicate") != string::npos) {
        auto pos = declaration.find("std::predicate");
        declaration.insert(pos, "std::negation<std::is_same<");
        declaration.insert(declaration.find(">"), ", std::nullptr_t>>::value");
      }
      llvm::outs() << declaration << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_468::MutatorASTConsumer_468::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = clang::ast_matchers::templateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}