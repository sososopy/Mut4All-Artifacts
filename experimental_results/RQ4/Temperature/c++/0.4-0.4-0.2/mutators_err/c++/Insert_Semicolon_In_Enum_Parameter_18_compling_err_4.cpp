//header file
#pragma once
#include "Mutator_base.h"

/**
 * insert_semicolon_in_enum_parameter_18
 */ 
class MutatorFrontendAction_18 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(18)

private:
    class MutatorASTConsumer_18 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_18(Rewriter &R) : TheRewriter(R) {}
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
#include "MutatorFrontendAction_18.h"

// ========================================================================================================
#define MUT18_OUTPUT 1

void MutatorFrontendAction_18::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithEnumParam")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
        return;
      
      for (unsigned i = 0; i < FD->getNumParams(); ++i) {
        if (FD->getParamDecl(i)->getType()->isEnumeralType()) {
          auto paramRange = FD->getParamDecl(i)->getSourceRange();
          std::string paramText = Lexer::getSourceText(CharSourceRange::getTokenRange(paramRange), 
                                                       *Result.SourceManager, 
                                                       Result.Context->getLangOpts()).str();
          size_t pos = paramText.find("enum");
          if (pos != std::string::npos) {
            paramText.insert(pos + 4, ";");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(paramRange), paramText);
          }
          break;
        }
      }
    }
}
  
void MutatorFrontendAction_18::MutatorASTConsumer_18::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasParameter(0, hasType(enumType()))).bind("FunctionWithEnumParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}