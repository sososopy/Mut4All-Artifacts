//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Template_Specialization_214
 */ 
class MutatorFrontendAction_214 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(214)

private:
    class MutatorASTConsumer_214 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_214(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::NamedDecl *> cur_typenames;
    };
};

//source file
#include "../include/Struct_Template_Specialization_214.h"

// ========================================================================================================
#define MUT214_OUTPUT 1

void MutatorFrontendAction_214::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TP = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      auto specs = TP->specializations();
      if (specs.empty())
        return;
      for (auto spec : specs) {
        if (spec->isExplicitSpecialization() == false)
          return;
      }
      cur_typenames.push_back(TP);
    } else if (auto *SP = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("Specializations")) {
      if (!SP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SP->getLocation()))
        return;
      if (cur_typenames.empty())
        return;
      auto TP = cur_typenames.back();
      auto args = SP->getTemplateArgs().asArray();
      if (args.size() == 0)
        return;
      auto arg0 = args[0];
      if (arg0.getKind() == clang::TemplateArgument::Type) {
        auto arg0_type = arg0.getAsType();
        for (auto cur_type : cur_typenames) {
          if (cur_type->getNameAsString() == arg0_type.getAsString()) {
            llvm::outs() << cur_type->getNameAsString() << '\n';
            auto new_spec = stringutils::rangetoStr(*(Result.SourceManager), SP->getSourceRange());
            new_spec = "/*mut214*/" + new_spec;
            new_spec = stringutils::replace_str(new_spec, arg0_type.getAsString(), "int");
            Rewrite.ReplaceText(SP->getEndLoc(), 0, new_spec);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_214::MutatorASTConsumer_214::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Templates");
    auto specialization_matcher = classTemplateSpecializationDecl().bind("Specializations");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(specialization_matcher, &callback);
    matchFinder.matchAST(Context);
}