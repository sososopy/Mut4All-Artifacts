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
      auto args = MT->getTemplateArgs().asArray();
      std::string new_instantiation = name + "<";
      for (std::size_t i = 0; i < args.size(); ++i) {
        if (args[i].getKind() ==
            clang::TemplateArgument::ArgKind::Integral) { // If the template
                                                          // argument is an
                                                          // integer
          auto val = args[i].getAsIntegral();
          llvm::outs() << val << '\n';
          llvm::SmallVector<char, 10> strbuf;
          val.toString(strbuf, 10, true);
          std::string str(strbuf.begin(), strbuf.end());
          new_instantiation += str;
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