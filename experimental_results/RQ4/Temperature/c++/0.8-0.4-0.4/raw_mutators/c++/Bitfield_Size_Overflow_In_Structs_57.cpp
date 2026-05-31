//header file
#pragma once
#include "Mutator_base.h"

/**
 * bitfield_size_overflow_in_structs_57
 */ 
class MutatorFrontendAction_57 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(57)

private:
    class MutatorASTConsumer_57 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_57(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/bitfield_size_overflow_in_structs_57.h"

// ========================================================================================================
#define MUT57_OUTPUT 1

void MutatorFrontendAction_57::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(SD->getLocation()))
        return;

      bool hasBitfield = false;
      for (auto *field : SD->fields()) {
          if (field->isBitField()) {
              hasBitfield = true;
              break;
          }
      }

      std::string structContent = stringutils::rangetoStr(*(Result.SourceManager), SD->getSourceRange());

      if (hasBitfield) {
          for (auto *field : SD->fields()) {
              if (field->isBitField()) {
                  std::string fieldDecl = stringutils::rangetoStr(*(Result.SourceManager), field->getSourceRange());
                  size_t colonPos = fieldDecl.find(":");
                  if (colonPos != std::string::npos) {
                      std::string newDecl = fieldDecl.substr(0, colonPos + 1) + " 1 + (unsigned long long)0xffffffffffffffff";
                      structContent.replace(structContent.find(fieldDecl), fieldDecl.length(), newDecl);
                  }
              }
          }
      } else {
          structContent.insert(structContent.rfind('}'), "int : 1 + (unsigned long long)0xffffffffffffffff;");
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SD->getSourceRange()), structContent);
    }
}
  
void MutatorFrontendAction_57::MutatorASTConsumer_57::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}