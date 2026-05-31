//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Static_Member_With_Incomplete_Type_159
 */ 
class MutatorFrontendAction_159 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(159)

private:
    class MutatorASTConsumer_159 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_159(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Static_Member_With_Incomplete_Type_159.h"

// ========================================================================================================
#define MUT159_OUTPUT 1

void MutatorFrontendAction_159::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;

      if (!ST->isStruct())
        return;

      auto static_fields = ST->decls();
      for (auto field : static_fields) {
        if (auto field_decl = llvm::dyn_cast<clang::FieldDecl>(field)) {
          if (field_decl->isStatic()) {
            auto field_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                     field->getSourceRange());
            field_str = "/*mut159*/static struct { } " +
                        field_decl->getNameAsString() + ";";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                    field->getSourceRange()),
                                field_str);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_159::MutatorASTConsumer_159::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Struct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}