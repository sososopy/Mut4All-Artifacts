//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Conceptual_Class_To_Template_With_Concept_88
 */ 
class MutatorFrontendAction_88 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(88)

private:
    class MutatorASTConsumer_88 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_88(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
        std::vector<const clang::CXXRecordDecl *> cur_concepts;
    };
};

//source file
#include "../include/change_conceptual_class_to_template_with_concept_88.h"

// ========================================================================================================
#define MUT88_OUTPUT 1

void MutatorFrontendAction_88::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      if (CL->isTemplated())
        return;
      if (CL->hasDefinition() == false)
        return;
      if (CL->isAbstract() == false)
        return;
      cur_classes.push_back(CL);
    } else if (auto *CP = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Concepts")) { // find concepts
      if (!CP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CP->getLocation()))
        return;
      if ((!CP->isStruct() && !CP->isClass()) || CP->isLambda())
        return;
      if (!CP->isCompleteDefinition())
        return;
      if (CP->isTemplated())
        return;
      if (CP->hasDefinition() == false)
        return;
      if (CP->isAbstract() == true)
        return;
      cur_concepts.push_back(CP);
    } else if (auto *TP = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Templates")) { // find templates
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      if ((!TP->isStruct() && !TP->isClass()) || TP->isLambda())
        return;
      if (!TP->isCompleteDefinition())
        return;
      if (TP->isTemplated() == false)
        return;
      return;
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>( // do
                                                                        // change
                 "Targets")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
        return;
      if (!DL->isCompleteDefinition())
        return;
      if (DL->isTemplated())
        return;
      if (DL->hasDefinition() == false)
        return;
      if (DL->isAbstract() == false)
        return;
      auto DLname = DL->getNameAsString();
      llvm::outs() << DLname << '\n';

      std::vector<int> candidate_index;
      for (std::size_t i = 0; i < cur_concepts.size(); ++i) {
        if (DL->isDerivedFrom(cur_concepts[i])) {
          candidate_index.push_back(i);
        }
      }
      if (candidate_index.empty())
        return;
      int choice = getrandom::getRandomIndex(candidate_index.size() - 1);
      int index = candidate_index[choice];
      auto concept_name = cur_concepts[index]->getNameAsString();
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      std::string template_head = "template<typename T,";
      template_head += concept_name;
      template_head += " C>";
      llvm::outs() << template_head << '\n';
      content = template_head + content;
      auto pos = content.find(DLname);
      if (pos != string::npos) {
        content.insert(pos + DLname.size(), "<T,C>");
      }
      auto fields = DL->fields();
      if (fields.begin() == fields.end())
        return;
      auto field_choice = getrandom::getRandomIndex(
          std::distance(fields.begin(), fields.end()) - 1);
      auto field = fields.begin();
      std::advance(field, field_choice);
      auto fieldname = field->getNameAsString();
      auto field_type = stringutils::rangetoStr(*(Result.SourceManager),
                                                field->getTypeSourceInfo()->getTypeLoc());
      llvm::outs() << fieldname << '\n';
      llvm::outs() << field_type << '\n';
      std::string newfield = "C mut_88{T()};\n";
      content.insert(content.rfind('}'), newfield);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_88::MutatorASTConsumer_88::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto concept_matcher = cxxRecordDecl().bind("Concepts");
    auto template_matcher = cxxRecordDecl().bind("Templates");
    auto target_matcher = cxxRecordDecl().bind("Targets");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(concept_matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.addMatcher(target_matcher, &callback);
    matchFinder.matchAST(Context);
}