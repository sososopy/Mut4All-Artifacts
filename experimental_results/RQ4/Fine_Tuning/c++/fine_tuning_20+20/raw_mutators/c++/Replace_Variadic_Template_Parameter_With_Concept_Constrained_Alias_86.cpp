//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Variadic_Template_Parameter_With_Concept_Constrained_Alias_86
 */ 
class MutatorFrontendAction_86 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(86)

private:
    class MutatorASTConsumer_86 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_86(Rewriter &R) : TheRewriter(R) {}
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
        const clang::TypeAliasDecl *cur_alias = nullptr;
        const clang::ConceptDecl *cur_concept = nullptr;
    };
};

//source file
#include "../include/replace_variadic_template_parameter_with_concept_constrained_alias_86.h"

// ========================================================================================================
#define MUT86_OUTPUT 1

void MutatorFrontendAction_86::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AL = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Alias")) {
      if (!AL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AL->getLocation()))
        return;
      if (AL->getNameAsString() == "U") {
        cur_alias = AL;
        llvm::outs() << "alias found\n";
      }
    } else if (auto *CP = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concept")) {
      if (!CP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CP->getLocation()))
        return;
      cur_concept = CP;
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                   "VariadicTemplate")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (cur_alias == nullptr || cur_concept == nullptr)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      auto concept_name = cur_concept->getNameAsString();
      auto alias_name = cur_alias->getNameAsString();
      llvm::outs() << concept_name << ' ' << alias_name << '\n';
      if (content.find("class...") != string::npos) {
        content.replace(content.find("class..."), 8, "class");
        if (content.find("B<T>...") != string::npos) {
          content.replace(content.find("B<T>..."), 7, "B<U>...");
          content = "/*mut86*/using U = " + concept_name + "<T>;\n" + content;
        }
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_86::MutatorASTConsumer_86::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto alias_matcher = typeAliasDecl().bind("Alias");
    auto concept_matcher = conceptDecl().bind("Concept");
    auto variadic_template_matcher =
        classTemplateDecl(has(cxxRecordDecl(hasDescendant(
                              cxxBaseSpecifier(hasType(hasDescendant(
                                  packExpansionType(hasDescendant(declRefExpr())))))))))
            .bind("VariadicTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(alias_matcher, &callback);
    matchFinder.addMatcher(concept_matcher, &callback);
    matchFinder.addMatcher(variadic_template_matcher, &callback);
    matchFinder.matchAST(Context);
}