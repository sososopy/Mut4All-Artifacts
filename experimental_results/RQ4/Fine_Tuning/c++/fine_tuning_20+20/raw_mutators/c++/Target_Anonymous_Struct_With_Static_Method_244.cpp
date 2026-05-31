//header file
#pragma once
#include "Mutator_base.h"

/**
 * Target_Anonymous_Struct_With_Static_Method_244
 */ 
class MutatorFrontendAction_244 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(244)

private:
    class MutatorASTConsumer_244 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_244(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::RecordDecl *> cur_structs;
    };
};

//source file
#include "../include/target_anonymous_struct_with_static_method_244.h"

// ========================================================================================================
#define MUT244_OUTPUT 1

void MutatorFrontendAction_244::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::RecordDecl>("Structs")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (!ST->isStruct())
        return;
      if (!ST->isAnonymousStructOrUnion())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             ST->getSourceRange());
      llvm::outs() << content << '\n';
      content += "/*mut244*/ static int mut_244();";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ST->getSourceRange()),
                          content);
      cur_structs.push_back(ST);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Classes")) { // If there is no anonymous struct in the seed
                                 // program, create a new one
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
        return;
      if (!cur_structs.empty())
        return;
      auto fields = DL->fields();
      if (fields.empty())
        return;
      auto field = fields.front();
      auto field_type = field->getType();
      if (field_type->isStructureType() == false)
        return;
      auto field_name = field->getNameAsString();
      auto ins = "/*mut244*/ struct {static int mut_244();} ";
      ins += field_name;
      ins += "_mut_244;";
      Rewrite.ReplaceText(field->getBeginLoc(), field_name.length(), ins);
    }
}

void MutatorFrontendAction_244::MutatorASTConsumer_244::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = recordDecl().bind("Structs");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}