//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transparent_Union_With_Nested_Union_229
 */ 
class MutatorFrontendAction_229 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(229)

private:
    class MutatorASTConsumer_229 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_229(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/transparent_union_with_nested_union_229.h"

// ========================================================================================================
#define MUT229_OUTPUT 1

void MutatorFrontendAction_229::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::UnionDecl>("Unions")) {
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;
      llvm::outs() << "Union name: " << UD->getNameAsString() << '\n';
      if (!UD->isCompleteDefinition())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), UD->getSourceRange());
      if (content.find("__attribute__((__transparent_union__))") !=
          string::npos)
        return;
      auto fields = UD->fields();
      bool has_nested = false;
      for (auto field : fields) {
        if (field->getType()->isUnionType()) {
          has_nested = true;
          auto nested_union = field->getType()->getAsUnionType()->getDecl();
          if (!nested_union->isCompleteDefinition())
            break;
          auto nested_name = nested_union->getNameAsString();
          llvm::outs() << "Nested name: " << nested_name << '\n';
          string nested_decl = "union " + nested_name;
          auto nested_content = stringutils::rangetoStr(
              *(Result.SourceManager), nested_union->getSourceRange());
          content = stringutils::replace_substr(content, nested_content,
                                                nested_decl);
          break;
        }
      }
      if (!has_nested) {
        if (content.rfind('}') != string::npos)
          content.insert(content.rfind('}'), "union B b;\n");
      }
      content += "__attribute__((__transparent_union__))";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(UD->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_229::MutatorASTConsumer_229::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = unionDecl().bind("Unions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}