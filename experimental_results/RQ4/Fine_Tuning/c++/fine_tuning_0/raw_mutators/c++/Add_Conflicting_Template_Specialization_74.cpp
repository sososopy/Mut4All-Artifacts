//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Conflicting_Template_Specialization_74
 */ 
class MutatorFrontendAction_74 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(74)

private:
    class MutatorASTConsumer_74 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_74(Rewriter &R) : TheRewriter(R) {}
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
        const ClassTemplateDecl *target = nullptr;
        std::string target_name;
        std::string target_concept;
        std::string target_type;
    };
};

//source file
#include "../include/Add_Conflicting_Template_Specialization_74.h"

// ========================================================================================================
#define MUT74_OUTPUT 1

void MutatorFrontendAction_74::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getBeginLoc()))
        return;
      if (TD->isThisDeclarationADefinition() == false)
        return;
      auto specs = TD->specializations();
      for (auto spec : specs) {
        if (spec->isExplicitSpecialization() == false)
          continue;
        if (spec->isCompleteDefinition() == false)
          continue;
        if (spec->getTemplateSpecializationKind() !=
            TemplateSpecializationKind::TSK_ExplicitSpecialization)
          continue;
        auto args = spec->getTemplateArgs();
        if (args.size() != 1)
          continue;
        auto arg = args[0];
        if (arg.getKind() != TemplateArgument::Type)
          continue;
        auto type = arg.getAsType();
        auto type_str = stringutils::rangetoStr(
            *(Result.SourceManager), type.getSourceRange());
        if (type_str.find("concept") == string::npos)
          continue;
        target = TD;
        target_name = TD->getNameAsString();
        target_concept = type_str;
        target_type = type_str.substr(type_str.find('<') + 1,
                                      type_str.find('>') - type_str.find('<') -
                                          1);
        llvm::outs() << target_name << '\n';
        llvm::outs() << target_concept << '\n';
        llvm::outs() << target_type << '\n';
      }
    } else if (auto *CD =
                   Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getBeginLoc()))
        return;
      if (target == nullptr)
        return;
      auto name = CD->getNameAsString();
      if (name != target_type)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             CD->getSourceRange());
      llvm::outs() << content << '\n';
      content = content.substr(0, content.rfind(';'));
      content += "&& false;";
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(CD->getSourceRange(), content);
      auto ins = "template<>" + target_name + "<" + target_concept +
                 "> struct " + target_name + "{};";
      llvm::outs() << ins << '\n';
      Rewrite.ReplaceText(target->getEndLoc(), 0, "/*mut74*/" + ins);
    }
}
  
void MutatorFrontendAction_74::MutatorASTConsumer_74::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Templates");
    auto concept_matcher = conceptDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(concept_matcher, &callback);
    matchFinder.matchAST(Context);
}