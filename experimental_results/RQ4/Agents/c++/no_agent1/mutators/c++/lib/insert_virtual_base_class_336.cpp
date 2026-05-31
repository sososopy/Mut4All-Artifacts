//source file
#include "../include/insert_virtual_base_class_336.h"

// ========================================================================================================
#define MUT336_OUTPUT 1

void MutatorFrontendAction_336::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Base = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!Base || !Result.Context->getSourceManager().isWrittenInMainFile(Base->getLocation()))
            return;
        if (!Base->isCompleteDefinition() || Base->isLambda())
            return;
        base_classes.push_back(Base);
    }
    else if (auto *Derived = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!Derived || !Result.Context->getSourceManager().isWrittenInMainFile(Derived->getLocation()))
            return;
        if (!Derived->isCompleteDefinition() || Derived->isLambda())
            return;

        if (base_classes.empty())
            return;

        // Choose a random base class to insert as a virtual base
        size_t index = getrandom::getRandomIndex(base_classes.size());
        const clang::CXXRecordDecl *base_class = base_classes[index];

        // Get the source code text of target node
        std::string derived_decl = stringutils::rangetoStr(*(Result.SourceManager), Derived->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string base_name = base_class->getNameAsString();
        std::string virtual_base = " : virtual " + base_name;
        if (derived_decl.find(':') == std::string::npos) {
            derived_decl.insert(derived_decl.find('{'), virtual_base);
        } else {
            derived_decl.insert(derived_decl.find('{'), ", virtual " + base_name);
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Derived->getSourceRange()), derived_decl);
    }
}

void MutatorFrontendAction_336::MutatorASTConsumer_336::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    auto base_matcher = cxxRecordDecl().bind("BaseClass");
    auto derived_matcher = cxxRecordDecl(isDerivedFrom(cxxRecordDecl())).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(base_matcher, &callback);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.matchAST(Context);
}