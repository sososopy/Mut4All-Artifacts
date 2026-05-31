//header file
#pragma once
#include "Mutator_base.h"

/**
 * Recursive_Template_Instantiation_26
 */ 
class MutatorFrontendAction_26 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(26)

private:
    class MutatorASTConsumer_26 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_26(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Recursive_Template_Instantiation_26.h"

// ========================================================================================================
#define MUT26_OUTPUT 1

void MutatorFrontendAction_26::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpecialization")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto name = MT->getNameAsString();
      llvm::outs() << name << '\n';
      auto args = MT->getTemplateArgs();
      std::string new_instantiation = name + "<";
      for (std::size_t i = 0; i < args.size(); ++i) {
        if (args[i].getKind() ==
            clang::TemplateArgument::ArgKind::Integral) { // If the template
                                                          // argument is an
                                                          // integer
          auto val = args[i].getAsIntegral();
          llvm::outs() << val << '\n';
          new_instantiation += val.toString(10);
        } else if (args[i].getKind() ==
                   clang::TemplateArgument::ArgKind::Type) { // If the template
                                                             // argument is a
                                                             // type
          auto type = args[i].getAsType();
          auto typestr = type.getAsString();
          llvm::outs() << typestr << '\n';
          new_instantiation += typestr;
        } else if (args[i].getKind() ==
                   clang::TemplateArgument::ArgKind::Template) { // If the
                                                                 // template
                                                                 // argument is
                                                                 // a template
          auto templateinfo = args[i].getAsTemplate();
          auto templatestr = templateinfo.getAsTemplateDecl()->getNameAsString();
          llvm::outs() << templatestr << '\n';
          new_instantiation += templatestr;
        } else {
          llvm::outs() << "unsupported template argument type" << '\n';
        }
        if (i != args.size() - 1)
          new_instantiation += ",";
      }
      new_instantiation += ">";
      llvm::outs() << new_instantiation << '\n';
      //Perform mutation on the source code text by applying string replacement
      new_instantiation =
          "/*mut26*/" + name + "<0," + new_instantiation + ">";
      llvm::outs() << new_instantiation << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), new_instantiation);
    }
}
  
void MutatorFrontendAction_26::MutatorASTConsumer_26::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}